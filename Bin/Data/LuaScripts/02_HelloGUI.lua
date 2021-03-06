-- A simple 'HelloWorld' GUI created purely from code.
-- This sample demonstrates:
--     - Creation of controls and building a UI hierarchy
--     - Loading UI style from XML and applying it to controls
--     - Handling of global and per-control events

require "LuaScripts/Utilities/Sample"

local window = nil

function Start()
    -- Execute the common startup for samples
    SampleStart()

    -- Enable OS cursor
    input.mouseVisible = true

    -- Load XML file containing default UI style sheet
    local style = cache:GetResource("XMLFile", "UI/DefaultStyle.xml")

    -- Set the loaded style as default style
    ui.root.defaultStyle = style

    -- Initialize Window
    InitWindow()

    -- Create and add some controls to the Window
    InitControls()

    SubscribeToEvents()
end

function InitControls()
    -- Create a CheckBox
    local checkBox = CheckBox:new()
    checkBox:SetName("CheckBox")

    -- Create a Button
    local button = Button:new()
    button:SetName("Button")
    button.minHeight = 24

    -- Create a LineEdit
    local lineEdit = LineEdit:new()
    lineEdit:SetName("LineEdit")
    lineEdit.minHeight = 24

    -- Add controls to Window
    window:AddChild(checkBox)
    window:AddChild(button)
    window:AddChild(lineEdit)

    -- Apply previously set default style
    checkBox:SetStyleAuto()
    button:SetStyleAuto()
    lineEdit:SetStyleAuto()
end

function InitWindow()
    -- Create the Window and add it to the UI's root node
    window = Window:new()
    ui.root:AddChild(window)
    
    -- Set Window size and layout settings
    window:SetMinSize(384, 192)
    window:SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6))
    window:SetAlignment(HA_CENTER, VA_CENTER)
    window:SetName("Window")
    
    -- Create Window 'titlebar' container
    local titleBar = UIElement:new()
    titleBar:SetMinSize(0, 24)
    titleBar.verticalAlignment = VA_TOP
    titleBar.layoutMode = LM_HORIZONTAL

    -- Create the Window title Text
    local windowTitle = Text:new()
    windowTitle.name = "WindowTitle"
    windowTitle.text = "Hello GUI!"
    
    
    -- Create the Window's close button
    local buttonClose = Button:new()
    buttonClose:SetName("CloseButton")

    -- Add the controls to the title bar
    titleBar:AddChild(windowTitle)
    titleBar:AddChild(buttonClose)

    -- Add the title bar to the Window
    window:AddChild(titleBar)
    
    
    -- Apply styles
    window:SetStyleAuto()
    windowTitle:SetStyleAuto()
    buttonClose:SetStyle("CloseButton")
    
    -- Lastly, subscribe to buttonClose release (following a 'press') events
    SubscribeToEvent(buttonClose, "Released", "HandleClosePressed")
end

function SubscribeToEvents()
    -- Subscribe handler invoked whenever a mouse click event is dispatched
    SubscribeToEvent("UIMouseClick", "HandleControlClicked")
end

function HandleClosePressed(eventType, eventData)
    engine:Exit()
end

function HandleControlClicked(eventType, eventData)
    -- Get the Text control acting as the Window's title
    local element = window:GetChild("WindowTitle", true)
    local windowTitle = tolua.cast(element, 'Text')
    
    -- Get control that was clicked
    -- Note difference to C++: in C++ we would call GetPtr() and cast the function pointer to UIElement, here we must specify
    -- what kind of object we are getting. Null will be returned on type mismatch
    local clicked = eventData:GetPtr("UIElement", "Element")
    local name = "...?"
    if clicked ~= nil then
        -- Get the name of the control that was clicked
        name = clicked.name
    end

    -- Update the Window's title text
    windowTitle.text = "Hello " .. name .. "!"
end
